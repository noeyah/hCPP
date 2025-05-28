# coding: utf-8

import sys
import os
import re

# protobuf 설치 : pip install protobuf
from google.protobuf import descriptor_pb2

# jinja2 설치 : pip install Jinja2
from jinja2 import Environment, FileSystemLoader


# desc 읽기 함수
def generate_enums(descriptor_file_path,    # protoc로 만든 .desc 파일 경로
                   start_id = 1             # PacketID 시작 번호 (None = 0 고정)
                   ):
    # desc 파일 읽기 + 파싱
    file_descriptor_set = descriptor_pb2.FileDescriptorSet()
    try:
        # 읽기 모드로 오픈해서 파싱
        with open(descriptor_file_path, "rb") as f:
            file_descriptor_set.ParseFromString(f.read())
    except Exception as e:
        print(f"Error : {e}", file = sys.stderr)
        sys.exit(1)

    # 패킷 정보
    file_names = []
    packet_ids = []
    req_packet_names = []
    res_packet_names = []
    not_packet_names = []
    enums_data = {}

    # Req, Res, Not 접미사만 패킷으로 판단
    packet_suffixes = ("Req", "Res", "Not")

    for proto_desc in file_descriptor_set.file:
        base_name = os.path.basename(proto_desc.name)
        name_without_ext = os.path.splitext(base_name)[0]
        file_names.append(name_without_ext)

        # 메시지 타입
        for message_type in proto_desc.message_type:
            msg_name = message_type.name

            if msg_name.endswith(packet_suffixes):
                packet_ids.append({
                    'name': msg_name
                })

            if msg_name.endswith("Req"):
                req_packet_names.append(msg_name)

            if msg_name.endswith("Res"):
                res_packet_names.append(msg_name)

            if msg_name.endswith("Not"):
                not_packet_names.append(msg_name)

        # enum 타입
        for enum_type in proto_desc.enum_type:
            enum_values = []
            for enum_value in enum_type.value:
                enum_values.append({
                    'name': enum_value.name,
                    'num': enum_value.number
                })
            enums_data[enum_type.name] = enum_values


    if not packet_ids:
        print("Error : PacketID 없음")
        return []

    file_names.sort()
    packet_ids.sort(key=lambda x: x['name'])
    req_packet_names.sort()
    res_packet_names.sort()
    not_packet_names.sort()

    current_id = start_id
    for packet_id in packet_ids:
        packet_id['num'] = current_id
        current_id += 1

    packet_info = {
        'file_names': file_names,
        'packet_ids': packet_ids,
        'req_names': req_packet_names,
        'res_names': res_packet_names,
        'not_names': not_packet_names,
        'enums': enums_data
        }

    return packet_info
# desc 읽기 함수 끝

# jinja 렌더 파일 생성 함수
def generate_jinja_render(packet_info,      # Packet 정보
                       jinja_template_dir,  # jinja 템플릿 폴더
                       output_dir           # 저장 경로
                       ):
    env = Environment(loader=FileSystemLoader(jinja_template_dir),
                      trim_blocks=True, lstrip_blocks=True)

    for file_name in os.listdir(jinja_template_dir):
        if file_name.endswith(".j2"):
            template = env.get_template(file_name)

            render_code = template.render(packet_info)

            # 파일명에서 .j2 제거해서 저장
            output_file_name = file_name[:-3]
            output_file_path = os.path.join(output_dir, output_file_name)
            try:
                os.makedirs(os.path.dirname(output_file_path), exist_ok=True)
                with open(output_file_path, "w", encoding = "utf-8-sig") as f:
                    f.write(render_code)
                print(f"Save {output_file_name}")
            except Exception as e:
                print(f"Error : Jinja Render in {jinja_template_dir} - {e}", file=sys.stderr)
                sys.exit(1)

    print(f"Success Jinja Render file {jinja_template_dir}")
# jinja 렌더 파일 생성 함수 끝


if __name__ == "__main__":
    
    if len(sys.argv) < 9:
        print("Error : argv", file = sys.stderr)
        sys.exit(1)

    desc_file = sys.argv[1]             # .desc 파일 경로
    packet_id_start = int(sys.argv[2])  # PacketID 시작 번호 (None = 0 고정)
    cpp_jinja_dir = sys.argv[3]         # c++ .j2 폴더
    cpp_jinja_out = sys.argv[4]         # c++ j2 렌더 파일 저장 폴더
    cs_jinja_dir = sys.argv[5]          # C# .j2 폴더
    cs_jinja_out = sys.argv[6]          # C# j2 렌더 파일 저장 폴더
    cpp_client_jinja_dir = sys.argv[7]  # c++ client j2 폴더
    cpp_client_jinja_out = sys.argv[8]  # c++ client j2 렌더 파일 저장 폴더
    
    # desc 파일로 PacketID 추출
    packet_info = generate_enums(desc_file, packet_id_start)

    if not packet_info:
        print("Error : 패킷 정보 없음")
        sys.exit(1)

    # j2 렌더
    generate_jinja_render(packet_info, cpp_jinja_dir, cpp_jinja_out)
    generate_jinja_render(packet_info, cs_jinja_dir, cs_jinja_out)
    generate_jinja_render(packet_info, cpp_client_jinja_dir, cpp_client_jinja_out)
